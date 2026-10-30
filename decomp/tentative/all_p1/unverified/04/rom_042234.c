/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042234.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42234(void)

{
  short sVar1;
  ushort uVar2;
  int unaff_A6;

  sVar1 = candidate_44926();
  uVar2 = sVar1 - *(short *)(unaff_A6 + 0x26);
  if ((short)uVar2 < 0) {
    uVar2 = -uVar2;
  }
  if ((uVar2 < 0x20) && (*(char *)(unaff_A6 + 0xa4) == '\0')) {
    candidate_4256();
    return;
  }
  return;
}
