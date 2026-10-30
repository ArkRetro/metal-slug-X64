/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ec1e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1ec1e(void)

{
  int iVar1;
  short sVar2;
  char cVar3;
  short sVar4;
  undefined1 unaff_D6b;
  short *psVar5;
  int unaff_A6;

  if (_DAT_0010000e != *(short *)(unaff_A6 + 0x7c)) {
    *(short *)(unaff_A6 + 0x7c) = _DAT_0010000e;
    if (*(short *)(unaff_A6 + 0x6e) == 0) {
      psVar5 = (short *)0x10c340;
    }
    else {
      psVar5 = (short *)0x10c348;
    }
    *(undefined1 *)(unaff_A6 + 0x70) = *(undefined1 *)(psVar5 + 2);
    cVar3 = *(char *)((int)psVar5 + 5);
    *(char *)((int)psVar5 + 5) = *(char *)((int)psVar5 + 5) + -1;
    *(short *)(unaff_A6 + 0x7a) = (short)cVar3;
    sVar4 = *psVar5;
    if ((*(byte *)(*(int *)(unaff_A6 + 0xc) + 0x15) & 1) == 0) {
      if (((*(byte *)(unaff_A6 + 0x15) & 1) != 0) && (*(char *)(unaff_A6 + 0x7a) < '\x01')) {
        *(undefined1 *)(psVar5 + 3) = 0;
      }
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
    }
    else {
      if (((*(byte *)(unaff_A6 + 0x15) & 1) == 0) && (*(char *)(unaff_A6 + 0x7a) < '\x01')) {
        *(undefined1 *)(psVar5 + 3) = 0;
      }
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 1;
      sVar4 = -sVar4;
    }
    sVar2 = psVar5[1];
    *(short *)(unaff_A6 + 0x7e) = sVar4;
    *(short *)(unaff_A6 + 0x80) = sVar2;
    iVar1 = *(int *)(unaff_A6 + 0xc);
    *(short *)(iVar1 + 0x2c) = *(short *)(iVar1 + 0x2c) + 1;
    *(short *)(iVar1 + 0x40) = *(short *)(iVar1 + 0x40) + 1;
    candidate_14a02();
    *(short *)(iVar1 + 0x2c) = *(short *)(iVar1 + 0x2c) + -1;
    *(short *)(iVar1 + 0x40) = *(short *)(iVar1 + 0x40) + -1;
    *(undefined1 *)(unaff_A6 + 0x84) = unaff_D6b;
    candidate_1ad6e();
    candidate_1ad52();
    candidate_1abc8();
    if (*(char *)(unaff_A6 + 0x70) == -1) {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 2;
    }
    else {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfd;
    }
  }
  return;
}
