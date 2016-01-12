/*
 * Copyright 2014-2016 Nippon Telegraph and Telephone Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 11805 2005-01-07 09:37:18Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "ifNumber.h"

#include "lagopus_apis.h"
#include "dataplane_interface.h"

void
count_ifNumber(int32_t *ifNumber) {
  lagopus_result_t ret = LAGOPUS_RESULT_ANY_FAILURES;
  size_t v;
  if (ifNumber != NULL) {
    struct port_stat *port_stat;
    if ((ret = dp_get_port_stat(&port_stat)) == LAGOPUS_RESULT_OK) {
      dataplane_count_ifNumber(port_stat, &v);
      *ifNumber = (int32_t)v;
      port_stat_release(port_stat);
      free(port_stat);
    } else {
      lagopus_msg_error("cannot count ports: %s",
                        lagopus_error_get_string(ret));
    }
  }
}

/** Initializes the ifNumber module */
void
init_ifNumber(void) {
  static oid ifNumber_oid[] = { 1,3,6,1,2,1,2,1 };

  DEBUGMSGTL(("ifNumber", "Initializing\n"));

  netsnmp_register_scalar(
    netsnmp_create_handler_registration("ifNumber",
                                        handle_ifNumber,
                                        ifNumber_oid,
                                        OID_LENGTH(ifNumber_oid),
                                        HANDLER_CAN_RONLY
                                       ));
}

int
handle_ifNumber(netsnmp_mib_handler *handler,
                netsnmp_handler_registration *reginfo,
                netsnmp_agent_request_info   *reqinfo,
                netsnmp_request_info         *requests) {
  /* We are never called for a GETNEXT if it's registered as a
     "instance", as it's "magically" handled for us.  */
  /* a instance handler also only hands us one request at a time, so
     we don't need to loop over a list of requests; we'll only get one. */
  int32_t v = 0;
  (void)handler;
  (void)reginfo;

  switch (reqinfo->mode) {
    case MODE_GET:
      count_ifNumber(&v);
      snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                               (u_char *) &v,
                               sizeof(v));
      break;
    default:
      /* we should never get here, so this is a really bad error */
      snmp_log(LOG_ERR, "unknown mode (%d) in handle_ifNumber\n", reqinfo->mode );
      return SNMP_ERR_GENERR;
  }
  return SNMP_ERR_NOERROR;
}
