/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0377f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_377f8(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar4;
  int unaff_A6;

  if (-1 < *(int *)(unaff_A6 + 0x72)) {
    uVar1 = *(short *)(*(int *)(unaff_A6 + 0x72) + 0x2c) - *(short *)(unaff_A6 + 0x2c);
    uVar2 = uVar1;
    if ((short)uVar1 < 0) {
      uVar2 = -uVar1;
    }
    uVar3 = 8;
    if (*(char *)(unaff_A6 + 0x98) == '\x06') {
      uVar3 = 0x10;
    }
    if (uVar3 < uVar2) {
      cVar4 = '\x01';
      if (-1 < (short)uVar1) {
        cVar4 = -1;
      }
      *(char *)(unaff_A6 + 0x98) = cVar4 + *(char *)(unaff_A6 + 0x98);
      candidate_377da();
      return;
    }
  }
  cVar4 = '\x01';
  if (*(byte *)(unaff_A6 + 0x98) != 6) {
    if (6 < *(byte *)(unaff_A6 + 0x98)) {
      cVar4 = -1;
    }
    *(char *)(unaff_A6 + 0x98) = cVar4 + *(char *)(unaff_A6 + 0x98);
  }
  return;
}
