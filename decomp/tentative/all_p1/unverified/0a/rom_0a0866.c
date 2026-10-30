/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0866.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0866(void)

{
  int iVar1;
  byte bVar2;
  undefined4 uVar3;
  int unaff_A6;

  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  if ((*(byte *)(iVar1 + 0x15) & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
  }
  *(short *)(unaff_A6 + 0x1e) = *(short *)(iVar1 + 0x1e) + -1;
  bVar2 = *(byte *)(unaff_A6 + 0x90) ^ *(byte *)(iVar1 + 0x90);
  *(undefined1 *)(unaff_A6 + 0x90) = *(undefined1 *)(iVar1 + 0x90);
  *(undefined1 *)(unaff_A6 + 0x8f) = *(undefined1 *)(iVar1 + 0x8f);
  if ((bVar2 & 0x20) == 0) {
    if ((bVar2 & 4) == 0) {
      if ((bVar2 & 2) != 0) {
        candidate_a0920();
      }
    }
    else {
      candidate_a0920();
    }
  }
  else if ((*(byte *)(unaff_A6 + 0x90) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  uVar3 = 0;
  if (((*(byte *)(unaff_A6 + 0x90) & 0x20) == 0) && (*(char *)(unaff_A6 + 0x8f) != '\0')) {
    uVar3 = 0xa0b02;
  }
  *(undefined4 *)(unaff_A6 + 0x50) = uVar3;
  return;
}
