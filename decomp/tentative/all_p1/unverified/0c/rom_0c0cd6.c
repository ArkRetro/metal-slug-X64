/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0cd6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000c0cdc: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000c0ce0) */

void candidate_c0cd6(void)

{
  int *extraout_A0;
  undefined1 in_CF;

  func_0x000040ac(0xc0ce0);
  if (((bool)in_CF) && (*extraout_A0 != 0xc10d8)) {
    *extraout_A0 = 0xc10fe;
    *(byte *)((int)extraout_A0 + 0x5f) = *(byte *)((int)extraout_A0 + 0x5f) | 0x80;
    *(undefined1 *)((int)extraout_A0 + 0x7b) = 0;
    *(undefined1 *)((int)extraout_A0 + 0x7a) = 0;
    candidate_1ae4a();
  }
  return;
}
