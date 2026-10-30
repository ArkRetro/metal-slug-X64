/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057aa6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_57aa6(void)

{
  short in_D0w;
  short sVar1;
  char *in_A0;
  char *in_A1;

  sVar1 = in_D0w + -1;
  if (in_D0w != 0) {
    do {
      if (*in_A0 != *in_A1) {
        return;
      }
      sVar1 = sVar1 + -1;
      in_A0 = in_A0 + 1;
      in_A1 = in_A1 + 1;
    } while (sVar1 != -1);
  }
  return;
}
