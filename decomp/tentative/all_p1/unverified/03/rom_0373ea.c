/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0373ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_373ea(void)

{
  char in_D0b;
  char cVar1;
  int *in_A0;

  cVar1 = '\0';
  do {
    if (*in_A0 == 0) {
      return;
    }
    cVar1 = cVar1 + '\x01';
    in_A0 = in_A0 + 1;
  } while (cVar1 != in_D0b);
  return;
}
