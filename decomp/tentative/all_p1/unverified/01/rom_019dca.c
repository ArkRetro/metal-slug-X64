/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019dca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_19dca(void)

{
  uint in_D0;
  int unaff_A5;

  if (8 < (byte)in_D0) {
    return 0;
  }
  return CONCAT31((int3)((in_D0 & 0xffff000f) >> 8),
                  *(undefined1 *)(unaff_A5 + 0x4326 + (int)(short)(in_D0 & 0xffff000f)));
}
