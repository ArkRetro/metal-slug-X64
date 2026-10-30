/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03012c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_3012c(void)

{
  char cVar1;
  short sVar2;
  char *in_A0;
  char *pcVar3;

  sVar2 = 0;
  while( true ) {
    pcVar3 = in_A0 + 1;
    cVar1 = *in_A0;
    if (cVar1 == '\0') break;
    if (cVar1 == '@') {
      in_A0 = in_A0 + 2;
    }
    else {
      in_A0 = pcVar3;
      if ((cVar1 != -0x22) && (cVar1 != -0x21)) {
        sVar2 = sVar2 + 1;
      }
    }
  }
  return sVar2;
}
