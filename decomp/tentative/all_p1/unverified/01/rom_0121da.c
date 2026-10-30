/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0121da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_121da(void)

{
  short sVar1;
  byte bVar2;
  char cVar3;
  int unaff_A5;

  cVar3 = DAT_00320000;
  DAT_0010bfee = DAT_00320000;
  bVar2 = *(byte *)(unaff_A5 + 0x3fed);
  *(byte *)(unaff_A5 + 0x3fed) = bVar2 & 0x7f;
  if ((bVar2 & 0x80) != 0) {
    DAT_00320000 = 3;
    return;
  }
  if (cVar3 == '\x01') {
    return;
  }
  bVar2 = DAT_0010f002 & 0x80;
  DAT_0010f002 = DAT_0010f002 & 0x7f;
  if (bVar2 != 0) {
    DAT_00320000 = 0xf;
    return;
  }
  sVar1 = *(short *)(unaff_A5 + 0x3fe8);
  if (sVar1 != *(short *)(unaff_A5 + 0x3fea)) {
    DAT_00320000 = *(char *)(unaff_A5 + 0x3f68 + (int)sVar1);
    _DAT_0010bfe8 = sVar1 + 1U & 0x7f;
  }
  return;
}
