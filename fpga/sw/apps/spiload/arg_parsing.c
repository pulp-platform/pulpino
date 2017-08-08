// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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

