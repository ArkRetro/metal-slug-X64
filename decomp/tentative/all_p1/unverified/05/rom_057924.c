/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057924.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0005793c) */

undefined1 candidate_57924(void)

{
  undefined1 in_D0b;
  char *in_A0;
  int in_A1;
  char *pcVar1;

  pcVar1 = (char *)(in_A1 + -1);
  *pcVar1 = *pcVar1 + in_A0[1];
  bcdAdjust(*pcVar1);
  pcVar1 = (char *)(in_A1 + -2);
  *pcVar1 = *pcVar1 + *in_A0;
  bcdAdjust(*pcVar1);
  pcVar1 = (char *)(in_A1 + -3);
  *pcVar1 = *pcVar1 + cRam000578fd;
  bcdAdjust(*pcVar1);
  pcVar1 = (char *)(in_A1 + -4);
  *pcVar1 = *pcVar1 + cRam000578fc;
  bcdAdjust(*pcVar1);
  return in_D0b;
}
