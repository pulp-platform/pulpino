/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

#include <argp.h>
#include "spiloader.h"

/* The options we understand. */
static struct argp_option options[] = {
  {"timeout", 't', "0", OPTION_ARG_OPTIONAL, "Timeout in seconds. 0 means no timeout" },
  { 0 }
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct cmd_arguments_t *arguments = (struct cmd_arguments_t*)state->input;

  switch (key)
  {
  case 't':
    if(arg)
      arguments->timeout = atoi(arg);
    break;

  case ARGP_KEY_ARG:
    arguments->stim = arg;
    break;

  case ARGP_KEY_INIT:
    // default values
    arguments->timeout = 0;
    break;

  case ARGP_KEY_FINI:
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, "spi_stim", "" };

void cmd_parsing(int argc, char* argv[], struct cmd_arguments_t* arguments)
{
  argp_parse (&argp, argc, argv, 0, 0, arguments);
}

