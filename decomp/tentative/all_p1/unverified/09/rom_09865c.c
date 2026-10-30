/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09865c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9865c(void)

{
  short sVar1;
  byte bVar2;
  short sVar3;
  undefined2 uVar4;
  undefined2 extraout_D1w;
  int unaff_A5;
  int unaff_A6;
  bool bVar5;

  bVar2 = *(byte *)(unaff_A6 + 0x92);
  bVar5 = bVar2 < 8;
  *(byte *)(unaff_A6 + 0x92) = bVar2 - 8;
  if (bVar5) {
    candidate_98b6e();
    if (bVar5) {
      sVar3 = 200;
      sVar1 = *(short *)(*(int *)(unaff_A6 + 0x6e) + 0x3c);
      if (*(short *)(unaff_A5 + 0x69b8) < sVar1) {
        sVar3 = -200;
      }
      *(short *)(unaff_A6 + 0x72) = sVar1 + sVar3;
      sVar3 = 0x60;
      sVar1 = *(short *)(*(int *)(unaff_A6 + 0x6e) + 0x40);
      if (0x180 < sVar1) {
        sVar3 = -0x60;
      }
      *(short *)(unaff_A6 + 0x74) = sVar1 + sVar3;
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x72) = 0x100;
      *(undefined2 *)(unaff_A6 + 0x74) = 0x180;
    }
    candidate_29c6();
    uVar4 = candidate_9901e();
    *(undefined2 *)(unaff_A6 + 0x72) = uVar4;
    *(undefined2 *)(unaff_A6 + 0x74) = extraout_D1w;
    bVar2 = *(byte *)(unaff_A6 + 0x8c);
    *(byte *)(unaff_A6 + 0x8c) = bVar2 & 0xfe;
    if ((bVar2 & 1) != 0) {
      *(byte *)(unaff_A6 + 0x91) = *(byte *)(unaff_A6 + 0x91) | 7;
    }
  }
  return;
}
