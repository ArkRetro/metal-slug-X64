/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca54a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ca54a(void)

{
  char cVar1;
  short unaff_D7w;
  short sVar2;
  char *in_A0;
  char *pcVar3;
  char *in_A1;

  sVar2 = unaff_D7w + -1;
  do {
    cVar1 = *in_A1;
    pcVar3 = in_A0 + 1;
    *in_A0 = cVar1;
    if (cVar1 == '\0') break;
    sVar2 = sVar2 + -1;
    in_A0 = pcVar3;
    in_A1 = in_A1 + 1;
  } while (sVar2 != -1);
  if (sVar2 != 0) {
    do {
      *pcVar3 = '\0';
      sVar2 = sVar2 + -1;
      pcVar3 = pcVar3 + 1;
    } while (sVar2 != -1);
  }
  return;
}
