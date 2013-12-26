<?php
/**
 * @file
 * merci_feature.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function merci_feature_default_rules_configuration() {
  $items = array();
  $items['rules_merci'] = entity_import('rules_config', '{ "rules_merci" : {
      "LABEL" : "Merci",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "merci_rules", "rules_forms" ],
      "ON" : { "rules_forms_reservation_node_form_form_validate" : [] },
      "IF" : [
        { "entity_is_of_type" : { "entity" : [ "entity" ], "type" : "node" } },
        { "merci_rules_condition_conflict" : {
            "entity" : [ "entity" ],
            "datetime_field" : "field_reservation_date",
            "entityreference_field" : "field_reservation_items"
          }
        }
      ],
      "DO" : [
        { "merci_rules_action_unavailable_entity_list" : {
            "USING" : {
              "entity" : [ "entity" ],
              "datetime_field" : "field_reservation_date",
              "entityreference_field" : "field_reservation_items"
            },
            "PROVIDE" : { "entity_object_list" : { "entity_object_list" : "List of conflicting reservation entity ids" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity-object-list" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "node", "id" : [ "list-item" ] },
                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                }
              },
              { "rules_forms_set_error" : {
                  "form" : [ "form" ],
                  "element" : "list_item",
                  "message" : "[entity-fetched:title] has  a conflict."
                }
              }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
