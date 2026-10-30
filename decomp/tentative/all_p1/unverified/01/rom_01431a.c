/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01431a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1431a(void)

{
  char *pcVar1;
  short in_D0w;
  short in_D1w;
  int in_A0;

  pcVar1 = *(char **)(in_A0 + 0x26);
  if (((ushort)((in_D0w >> 3) - (short)*pcVar1) < (ushort)(byte)pcVar1[2]) &&
     ((ushort)~(((in_D1w >> 3) - (short)pcVar1[1]) - (ushort)(byte)pcVar1[3]) <
      (ushort)(byte)pcVar1[3])) {
    return;
  }
  return;
}
