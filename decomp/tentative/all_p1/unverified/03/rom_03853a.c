/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03853a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3853a(void)

{
  ushort uVar1;
  short sVar2;
  char cVar3;
  int unaff_A6;

  uVar1 = *(short *)(unaff_A6 + 0x28) - 0x128;
  if ((short)uVar1 < 0) {
    uVar1 = -uVar1;
  }
  *(short *)(unaff_A6 + 0x2c) = (short)(((uint)uVar1 * 0x32) / 0x128) + *(short *)(unaff_A6 + 0x90);
  sVar2 = *(short *)(unaff_A6 + 0x28) + -0x128;
  if (-1 < *(short *)(unaff_A6 + 0x30)) {
    sVar2 = -sVar2;
  }
  cVar3 = '\x01';
  if (sVar2 < 0) {
    cVar3 = -1;
  }
  *(char *)(unaff_A6 + 0x98) = cVar3 + *(char *)(unaff_A6 + 0x98);
  candidate_377da();
  candidate_37854();
  candidate_1ac38();
  return;
}
