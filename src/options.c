/**
 * options.c -- functions related to parsing program options
 * Copyright (C) 2009-2014 by Gerardo Orellana <goaccess@prosoftcorp.com>
 * GoAccess - An Ncurses apache weblog analyzer & interactive viewer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the GNU General Public License is attached to this
 * source distribution for its full text.
 *
 * Visit http://goaccess.prosoftcorp.com for new releases.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#ifdef HAVE_LIBTOKYOCABINET
#include "tcabinet.h"
#endif

#include "options.h"

#include "error.h"
#include "settings.h"
#include "util.h"

static char short_options[] = "f:e:p:o:"
#ifdef DEBUG
  "l:"
#endif
#ifdef HAVE_LIBGEOIP
  "g"
#endif
  "acrmMhHqdsV";

/* *INDENT-OFF* */
struct option long_opts[] = {
  {"agent-list"           , no_argument       , 0 , 'a' } ,
  {"config-dialog"        , no_argument       , 0 , 'c' } ,
  {"config-file"          , required_argument , 0 , 'p' } ,
  {"exclude-ip"           , required_argument , 0 , 'e' } ,
  {"help"                 , no_argument       , 0 , 'h' } ,
  {"http-method"          , no_argument       , 0 , 'M' } ,
  {"http-protocol"        , no_argument       , 0 , 'H' } ,
  {"log-file"             , required_argument , 0 , 'f' } ,
  {"version"              , no_argument       , 0 , 'V' } ,
#ifdef DEBUG
  {"debug-file"           , required_argument , 0 , 'l' } ,
#endif
  {"444-as-404"           , no_argument       , 0 ,  0  } ,
  {"4xx-to-unique-count"  , no_argument       , 0 ,  0  } ,
  {"color-scheme"         , required_argument , 0 ,  0  } ,
  {"date-format"          , required_argument , 0 ,  0  } ,
  {"ignore-crawlers"      , no_argument       , 0 ,  0  } ,
  {"ignore-referer"       , required_argument , 0 ,  0  } ,
  {"log-format"           , required_argument , 0 ,  0  } ,
  {"sort-view"            , required_argument , 0 ,  0  } ,
  {"no-color"             , no_argument       , 0 ,  0  } ,
  {"no-global-config"     , no_argument       , 0 ,  0  } ,
  {"no-progress"          , no_argument       , 0 ,  0  } ,
  {"no-query-string"      , no_argument       , 0 , 'q' } ,
  {"no-term-resolver"     , no_argument       , 0 , 'r' } ,
  {"output-format"        , required_argument , 0 , 'o' } ,
  {"real-os"              , no_argument       , 0 ,  0  } ,
  {"static-file"          , required_argument , 0 ,  0  } ,
  {"storage"              , no_argument       , 0 , 's' } ,
  {"with-mouse"           , no_argument       , 0 , 'm' } ,
  {"with-output-resolver" , no_argument       , 0 , 'd' } ,
#ifdef HAVE_LIBGEOIP
  {"std-geoip"            , no_argument       , 0 , 'g' } ,
  {"geoip-city-data"      , required_argument , 0 ,  0  } ,
#endif
#ifdef TCB_BTREE
  {"cache-lcnum"          , required_argument , 0 ,  0  } ,
  {"cache-ncnum"          , required_argument , 0 ,  0  } ,
  {"compression"          , required_argument , 0 ,  0  } ,
  {"db-path"              , required_argument , 0 ,  0  } ,
  {"keep-db-files"        , no_argument       , 0 ,  0  } ,
  {"load-from-disk"       , no_argument       , 0 ,  0  } ,
  {"tune-bnum"            , required_argument , 0 ,  0  } ,
  {"tune-lmemb"           , required_argument , 0 ,  0  } ,
  {"tune-nmemb"           , required_argument , 0 ,  0  } ,
  {"xmmap"                , required_argument , 0 ,  0  } ,
#endif
  {0, 0, 0, 0}
};

void
cmd_help (void)
{
  printf ("\nGoAccess - %s\n\n", GO_VERSION);
  printf (
  "Usage: "
  "goaccess [ options ... ] -f log_file [-c][-M][-H][-q][-d][...]\n"
  "The following options can also be supplied to the command:\n\n"

  /* Log & Date Format Options */
  "Log & Date Format Options\n\n"
  "  --date-format=<dateformat>  - Specify log date format.\n"
  "  --log-format=<logformat>    - Specify log format. Inner quotes need to\n"
  "                                be escaped.\n\n"

  /* User Interface Options */
  "User Interface Options\n\n"
  "  -c --config-dialog          - Prompt log/date configuration window.\n"
  "  --color-scheme=<1|2>        - Color schemes: 1 => Grey, 2 => Green.\n"
  "  --no-color                  - Disable colored output.\n\n"

  /* File Options */
  "File Options\n\n"
  "  -f --log-file=<filename>    - Path to input log file.\n"
  "  -p --config-file=<filename> - Custom configuration file.\n"
#ifdef DEBUG
  "  -l --debug-file=<filename>  - Send all debug messages to the\n"
  "                                specified file.\n"
#endif
  "  --no-global-config          - Don't load global configuration file.\n\n"

  /* Parse Options */
  "Parse Options\n\n"
  "  -a --agent-list             - Enable a list of user-agents by host.\n"
  "  -d --with-output-resolver   - Enable IP resolver on HTML|JSON output.\n"
  "  -e --exclude-ip=<IP>        - Exclude one or multiple IPv4/6, includes\n"
  "                                IP ranges. e.g., 192.168.0.1-192.168.0.10\n"
  "  -H --http-protocol          - Include HTTP request protocol if found.\n"
  "  -M --http-method            - Include HTTP request method if found.\n"
  "  -m --with-mouse             - Enable mouse support on main dashboard.\n"
  "  -o --output-format=csv|json - Output either a JSON or a CSV file.\n"
  "  -q --no-query-string        - Ignore request's query string.\n"
  "  -r --no-term-resolver       - Disable IP resolver on terminal output.\n"
  "  --444-as-404                - Treat non-standard status code 444 as 404.\n"
  "  --4xx-to-unique-count       - Add 4xx client errors to the unique\n"
  "                                visitors count.\n"
  "  --ignore-crawlers           - Ignore crawlers.\n"
  "  --ignore-referer=<needle>  - Ignore a referer from being counted.\n"
  "                                Wild cards are allowed. i.e., *.bing.com\n"
  "  --no-progress               - Disable progress metrics.\n\n"
  "  --real-os                   - Display real OS names. e.g, Windows XP,\n"
  "                                Snow Leopard.\n"
  "  --sort-view=MOD,FIELD,ORDER - Sort panel on initial load. For example:\n"
  "                                --sort-view=VISITORS,BY_HITS,ASC\n"
  "                                See manpage for a list of panels and fields.\n"
  "  --static-file=<extension>   - Add static file extension. e.g.: .mp3\n"
  "                                Extensions are case sensitive.\n"

/* GeoIP Options */
#ifdef HAVE_LIBGEOIP
  "GeoIP Options\n\n"
  "  -g --std-geoip              - Standard GeoIP database for less memory\n"
  "                                usage.\n"
  "  --geoip-city-data=<path>    - Specify path to GeoIP City database file.\n"
  "                                i.e., GeoLiteCity.dat\n\n"
#endif

/* On-Disk Database Options */
#ifdef TCB_BTREE
  "On-Disk Database Options\n\n"
  "  --keep-db-files             - Persist parsed data into disk.\n"
  "  --load-from-disk            - Load previously stored data from disk.\n"
  "  --db-path=<path>            - Path of the database file. Default [%s]\n"
  "  --xmmap=<number>            - Set the size in bytes of the extra\n"
  "                                mapped memory. Default [%d]\n"
  "  --cache-lcnum=<number>      - Max number of leaf nodes to be cached.\n"
  "                                Default [%d]\n"
  "  --cache-ncnum=<number>      - Max number of non-leaf nodes to be cached.\n"
  "                                Default [%d]\n"
  "  --tune-lmemb=<number>       - Number of members in each leaf page.\n"
  "                                Default [%d]\n"
  "  --tune-nmemb=<number>       - Number of members in each non-leaf page.\n"
  "                                Default [%d]\n"
  "  --tune-bnum=<number>        - Number of elements of the bucket array.\n"
  "                                Default [%d]\n"
#if defined(HAVE_ZLIB) || defined(HAVE_BZ2)
  "  --compression=<zlib|bz2>    - Specifies that each page is compressed\n"
  "                                with ZLIB|BZ2 encoding.\n\n"
#endif
#endif

/* Other Options */
  "Other Options\n\n"
  "  -h --help                   - This help.\n"
  "  -V --version                - Display version information and exit.\n"
  "  -s --storage                - Display current storage method.\n"
  "                                e.g., B+ Tree, Hash.\n\n"

  "Examples can be found by running `man goaccess`.\n\n"
  "For more details visit: http://goaccess.io\n"
  "GoAccess Copyright (C) 2009-2014 GNU GPL'd, by Gerardo Orellana"
  "\n\n"
#ifdef TCB_BTREE
  , TC_DBPATH, TC_MMAP, TC_LCNUM, TC_NCNUM, TC_LMEMB, TC_NMEMB, TC_BNUM
#endif
  );
  exit (EXIT_FAILURE);
}
/* *INDENT-ON* */

void
verify_global_config (int argc, char **argv)
{
  int o, idx = 0;

  conf.load_global_config = 1;
  while ((o = getopt_long (argc, argv, short_options, long_opts, &idx)) >= 0) {
    if (-1 == o || EOF == o)
      break;

    switch (o) {
    case 'p':
      conf.iconfigfile = optarg;
      break;
    case 0:
      if (!strcmp ("no-global-config", long_opts[idx].name))
        conf.load_global_config = 0;
      break;
    case '?':
      exit (EXIT_FAILURE);
    }
  }
  for (idx = optind; idx < argc; idx++)
    cmd_help ();

  /* reset it to 1 */
  optind = 1;
}

void
read_option_args (int argc, char **argv)
{
  int o, idx = 0;

#ifdef HAVE_LIBGEOIP
  conf.geo_db = GEOIP_MEMORY_CACHE;
#endif

  while ((o = getopt_long (argc, argv, short_options, long_opts, &idx)) >= 0) {
    if (-1 == o || EOF == o)
      break;
    switch (o) {
    case 'f':
      conf.ifile = optarg;
      break;
    case 'p':
      /* ignore it */
      break;
#ifdef HAVE_LIBGEOIP
    case 'g':
      conf.geo_db = GEOIP_STANDARD;
      break;
#endif
    case 'e':
      conf.ignore_ips[conf.ignore_ip_idx++] = optarg;
      break;
    case 'a':
      conf.list_agents = 1;
      break;
    case 'c':
      conf.load_conf_dlg = 1;
      break;
    case 'q':
      conf.ignore_qstr = 1;
      break;
    case 'o':
      conf.output_format = optarg;
      break;
    case 'l':
      conf.debug_log = optarg;
      dbg_log_open (conf.debug_log);
      break;
    case 'r':
      conf.skip_term_resolver = 1;
      break;
    case 'd':
      conf.enable_html_resolver = 1;
      break;
    case 'm':
      conf.mouse_support = 1;
      break;
    case 'M':
      conf.append_method = 1;
      break;
    case 'h':
      cmd_help ();
      break;
    case 'H':
      conf.append_protocol = 1;
      break;
    case 'V':
      display_version ();
      exit (EXIT_SUCCESS);
      break;
    case 0:

      if (!strcmp ("no-global-config", long_opts[idx].name))
        break;  /* ignore it */

      /* color scheme */
      if (!strcmp ("color-scheme", long_opts[idx].name))
        conf.color_scheme = atoi (optarg);

      /* log format */
      if (!strcmp ("log-format", long_opts[idx].name))
        conf.log_format = unescape_str (optarg);

      /* date format */
      if (!strcmp ("date-format", long_opts[idx].name))
        conf.date_format = unescape_str (optarg);

      /* static file */
      if (!strcmp ("static-file", long_opts[idx].name)) {
        if (conf.static_file_max_len < strlen (optarg))
          conf.static_file_max_len = strlen (optarg);
        conf.static_files[conf.static_file_idx++] = optarg;
      }
      /* 4xx to unique count */
      if (!strcmp ("4xx-to-unique-count", long_opts[idx].name))
        conf.client_err_to_unique_count = 1;

      /* 444 as 404 */
      if (!strcmp ("444-as-404", long_opts[idx].name))
        conf.code444_as_404 = 1;

      /* ignore crawlers */
      if (!strcmp ("ignore-crawlers", long_opts[idx].name))
        conf.ignore_crawlers = 1;

      /* ignore referer */
      if (!strcmp ("ignore-referer", long_opts[idx].name))
        conf.ignore_referers[conf.ignore_referer_idx++] = optarg;

      /* sort view */
      if (!strcmp ("sort-view", long_opts[idx].name))
        conf.sort_views[conf.sort_view_idx++] = optarg;

      /* real os */
      if (!strcmp ("real-os", long_opts[idx].name))
        conf.real_os = 1;

      /* no color */
      if (!strcmp ("no-color", long_opts[idx].name))
        conf.no_color = 1;

      /* no progress */
      if (!strcmp ("no-progress", long_opts[idx].name))
        conf.no_progress = 1;

      /* specifies the path of the GeoIP City database file */
      if (!strcmp ("geoip-city-data", long_opts[idx].name))
        conf.geoip_city_data = optarg;

      /* load data from disk */
      if (!strcmp ("load-from-disk", long_opts[idx].name))
        conf.load_from_disk = 1;

      /* keep database files */
      if (!strcmp ("keep-db-files", long_opts[idx].name))
        conf.keep_db_files = 1;

      /* specifies the path of the database file */
      if (!strcmp ("db-path", long_opts[idx].name))
        conf.db_path = optarg;

      /* set the size in bytes of the extra mapped memory */
      if (!strcmp ("xmmap", long_opts[idx].name))
        conf.xmmap = atoi (optarg);

      /* specifies the maximum number of leaf nodes to be cached */
      if (!strcmp ("cache-lcnum", long_opts[idx].name))
        conf.cache_lcnum = atoi (optarg);

      /* specifies the maximum number of non-leaf nodes to be cached */
      if (!strcmp ("cache-ncnum", long_opts[idx].name))
        conf.cache_ncnum = atoi (optarg);

      /* number of members in each leaf page */
      if (!strcmp ("tune-lmemb", long_opts[idx].name))
        conf.tune_lmemb = atoi (optarg);

      /* number of members in each non-leaf page */
      if (!strcmp ("tune-nmemb", long_opts[idx].name))
        conf.tune_nmemb = atoi (optarg);

      /* number of elements of the bucket array */
      if (!strcmp ("tune-bnum", long_opts[idx].name))
        conf.tune_bnum = atoi (optarg);

      /* specifies that each page is compressed with X encoding */
      if (!strcmp ("compression", long_opts[idx].name)) {
#ifdef HAVE_ZLIB
        if (!strcmp ("zlib", optarg))
          conf.compression = TC_ZLIB;
#endif
#ifdef HAVE_BZ2
        if (!strcmp ("bz2", optarg))
          conf.compression = TC_BZ2;
#endif
      }

      break;
    case 's':
      display_storage ();
      exit (EXIT_SUCCESS);
    case '?':
      exit (EXIT_FAILURE);
    default:
      exit (EXIT_FAILURE);
    }
  }

  for (idx = optind; idx < argc; idx++)
    cmd_help ();
}
