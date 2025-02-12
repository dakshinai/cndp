/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2019-2022 Intel Corporation.
 */

#ifndef _FWD_H_
#define _FWD_H_

/**
 * @file
 *
 * CNE Graph Node example code.
 *
 * A simple l3fwd example using the graph node libraries.
 */

#include <stdint.h>        // for uint64_t, uint32_t
#include <sys/types.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <strings.h>        // for strcasecmp
#include <stdbool.h>        // for bool
#include <pthread.h>        // for pthread_barrier_t

#ifdef __cplusplus
extern "C" {
#endif

#include <cne_graph.h>        // for cne_graph_t
#include <jcfg.h>             // for jcfg_info_t
#include <jcfg_process.h>

#include "metrics.h"        // for metrics_info_t
#include "pktmbuf.h"        // for pktmbuf_t

#define MAX_THREADS 16
#define MAX_BURST   256
#define DST_LPORT   5

enum {
    FWD_DEBUG_STATS = (1 << 0), /**< Show debug stats */
    FWD_NO_METRICS  = (1 << 1), /**< Disable the Metrics function */
    FWD_NO_RESTAPI  = (1 << 2), /**< Disable the REST API */
    FWD_CLI_ENABLE  = (1 << 3), /**< Enable the CLI */
};

#define NO_METRICS_TAG "no-metrics" /**< json tag for no-metrics */
#define NO_RESTAPI_TAG "no-restapi" /**< json tag for no-restapi */
#define ENABLE_CLI_TAG "cli"        /**< json tag to enable/disable CLI */

struct fwd_port {
    int lport;                      /**< PKTDEV lport id */
    pktmbuf_t *rx_mbufs[MAX_BURST]; /**< RX mbufs array */
    uint64_t ipackets;              /**< previous rx packets */
    uint64_t opackets;              /**< previous tx packets */
    uint64_t ibytes;                /**< previous rx bytes */
    uint64_t obytes;                /**< previous tx bytes */
};

struct app_options {
    bool no_metrics; /**< Enable metrics*/
    bool no_restapi; /**< Enable REST API*/
    bool cli;        /**< Enable Cli*/
    unsigned int node_cnt;
    unsigned int node_sz;
    const char **nodes;
};

typedef struct graph_info_s {
    cne_graph_t id;
    struct cne_graph *graph;
    int cnt;
    int nb_patterns;
    const char **patterns;
} graph_info_t;

struct fwd_info {
    jcfg_info_t *jinfo;        /**< JSON-C configuration */
    uint32_t flags;            /**< Application set of flags */
    volatile int timer_quit;   /**< flags to start and stop the application */
    struct app_options opts;   /**< Application options*/
    pthread_barrier_t barrier; /**< Barrier for all threads */
    bool barrier_inited;
    graph_info_t graph_info[16];
};

extern struct fwd_info *fwd; /**< global application informatio pointer */

int parse_args(int argc, char **argv);
void thread_func(void *arg);
int enable_metrics(void);
void print_stats(void *arg);
void print_port_stats_all(void);

#ifdef __cplusplus
}
#endif

#endif /* _FWD_H_ */
