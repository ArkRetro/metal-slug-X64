/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6dba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a6dba(void)

{
  int in_A0;
  char *pcVar1;
  int unaff_A6;

  pcVar1 = *(char **)(in_A0 + (short)((*(byte *)(unaff_A6 + 0x15) & 1) * 4));
  *(char *)(unaff_A6 + 0x7a) = *(char *)(unaff_A6 + 0x7a) + '\x01';
  while (*pcVar1 != -1) {
    candidate_a6de4();
    pcVar1 = pcVar1 + 1;
  }
  return;
}
