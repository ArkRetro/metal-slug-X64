/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05796e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x00057980) */

undefined1 candidate_5796e(void)

{
  undefined1 in_D0b;
  char *in_A0;
  int in_A1;
  char *pcVar1;

  pcVar1 = (char *)(in_A1 + -1);
  *pcVar1 = *pcVar1 + in_A0[3];
  bcdAdjust(*pcVar1);
  pcVar1 = (char *)(in_A1 + -2);
  *pcVar1 = *pcVar1 + in_A0[2];
  bcdAdjust(*pcVar1);
  pcVar1 = (char *)(in_A1 + -3);
  *pcVar1 = *pcVar1 + in_A0[1];
  bcdAdjust(*pcVar1);
  pcVar1 = (char *)(in_A1 + -4);
  *pcVar1 = *pcVar1 + *in_A0;
  bcdAdjust(*pcVar1);
  return in_D0b;
}
