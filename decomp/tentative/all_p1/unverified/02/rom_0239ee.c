/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0239ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_239ee(void)

{
  int iVar1;
  undefined4 uVar2;
  byte bVar3;
  short sVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x24) == 3) {
    iVar1 = unaff_A6[3];
    sVar4 = *(short *)(iVar1 + 0x9a) * 4;
    if ((*(byte *)(iVar1 + 0x8d) & 8) == 0) {
      unaff_A6[0x14] = *(undefined4 *)(sVar4 + 0xe0098);
    }
    else {
      unaff_A6[0x14] = 0;
    }
    if (*(short *)(iVar1 + 0x9a) == 9) {
      sVar4 = sVar4 + -2;
    }
    uVar2 = unaff_A6[0xf];
    *(short *)(unaff_A6 + 0xf) = sVar4 + *(short *)(unaff_A6 + 0xf);
    candidate_427a();
    unaff_A6[0xf] = uVar2;
  }
  else {
    if ((*(byte *)(unaff_A6[3] + 0x8d) & 8) == 0) {
      unaff_A6[0x14] = 0xe00c0;
    }
    else {
      unaff_A6[0x14] = 0;
    }
    candidate_427a();
  }
  uVar5 = false;
  if ((*(char *)(unaff_A6[3] + 0x8e) != '\0') &&
     (bVar3 = *(byte *)(unaff_A6[3] + 0x8b), uVar5 = *(byte *)((int)unaff_A6 + 0x8b) < bVar3,
     *(byte *)((int)unaff_A6 + 0x8b) == bVar3)) {
    *unaff_A6 = candidate_23ae6;
    uVar5 = false;
  }
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_24164;
    uVar5 = 0;
  }
  candidate_24c94();
  candidate_24a94();
  if ((bool)uVar5) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_240ee;
  }
  return;
}
