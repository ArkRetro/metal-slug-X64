#ifndef MSX_MARCO_COMMON_H
#define MSX_MARCO_COMMON_H
#include "marco_body.h"
/* Neutral names preserve original field widths without reproducing RAM layout. */
typedef enum {MCC_KEEP,MCC_DEATH_2956,MCC_FALL_DEATH_2D6A} marco_common_destination_t;
typedef struct {
 uint16_t timer84;
 uint8_t field5a,field19,field1a,field94,field9e,field99;
 int16_t projected_y,death_line;
 bool global_invincible,alternate_palette;
 marco_common_destination_t destination;
} marco_common_t;
typedef enum {MCQ_PROTECTION_CA2F8,MCQ_CONTACT_4C24,MCQ_PROXIMITY_49E8} marco_common_query_t;
typedef enum {MCP_35FC,MCP_3614,MCP_361C} marco_common_palette_t;
typedef struct {
 void *context;
 bool (*query)(void*,marco_common_query_t,marco_body_t*,uint16_t*,bool*);
 bool (*prepare)(void*,marco_common_t*,marco_body_t*,marco_animation_t*); /* 019432 */
 bool (*palette_lookup)(void*,marco_common_palette_t,uint16_t,uint16_t*);
 bool (*palette_write)(void*,uint8_t,uint16_t); /* 009D1E */
 bool (*reset_weapon)(void*,marco_body_t*); /* C0EE2: not just clearing a clock. */
} marco_common_services_t;
/* C32F4..C3348: world preparation, variant79, native upper controller, flags.
 * The owner must next execute C3348..C3634, then common_tail; this is not a
 * complete common update and must not be followed directly by a frame commit. */
bool marco_common_begin(marco_common_t*,marco_body_t*,marco_animation_t*,
 const marco_common_services_t*,const marco_upper_services_t*);
/* C35D0..C3634: palette countdown after form transitions. */
bool marco_common_palette(marco_common_t*,marco_body_t*,marco_animation_t*,const marco_common_services_t*);
/* C3634..C3776 only. Preceding form transitions, palette countdown and upper
 * update must already have run. Destinations are scheduled, never executed here.
 * No missing service is assumed to return a harmless collision result. */
bool marco_common_tail(marco_common_t*,marco_body_t*,marco_animation_t*,const marco_common_services_t*);
#endif
