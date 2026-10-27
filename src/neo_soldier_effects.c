#include "neo_soldier_effects.h"
bool neo_soldier_animation_event(void *context,neo_anim_kind_t kind,uint32_t asset,
    neo_anim_render_t *render) {
    neo_soldier_effects_t *effects=context;
    if (!effects) return false;
    if (kind==NEO_ANIM_REPLACE_PALETTE) {
        uint8_t handle;
        if (asset!=0x35c || !effects->replace_palette ||
            !effects->replace_palette(effects->context,(uint16_t)asset,&handle)) return false;
        render->palettes[0]=handle;render->attributes=(uint16_t)((render->attributes&255u)|((unsigned)handle<<8));
        return true;
    }
    if (kind==NEO_ANIM_SPAWN_THROW) {
        if (asset!=0x3bc28 || !effects->source || !effects->scene || !effects->enqueue) return false;
        neo_contact_actor_t child;
        neo_throw_spawn(&child,effects->source,effects->scene,effects->epoch);
        return effects->enqueue(effects->context,&child,asset,0);
    }
    return false;
}
