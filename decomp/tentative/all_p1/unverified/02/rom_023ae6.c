/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x023ae6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_23ae6(void)

{
  int iVar1;
  undefined4 uVar2;
  byte bVar3;
  short sVar4;
  undefined2 uVar5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(unaff_A6[3] + 0x72);
  *(undefined1 *)((int)unaff_A6 + 0x8a) = *(undefined1 *)(unaff_A6[3] + 0x8a);
  if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
    uVar5 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar5;
    uVar5 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar5;
  }
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x01') {
    uVar5 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x92) = uVar5;
    uVar5 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x26) = uVar5;
  }
  if (*(short *)(unaff_A6 + 0x26) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    candidate_40bc();
    *unaff_A6 = 0x23b86;
    candidate_1abc8();
    uVar6 = *(ushort *)(unaff_A6 + 0x24) < 3;
    if (*(ushort *)(unaff_A6 + 0x24) == 3) {
      sVar4 = *(short *)(unaff_A6[3] + 0x9a) * 4;
      unaff_A6[0x14] = *(undefined4 *)(sVar4 + 0xe0098);
      if (*(short *)(unaff_A6[3] + 0x9a) == 9) {
        sVar4 = sVar4 + -2;
      }
      uVar2 = unaff_A6[0xf];
      *(short *)(unaff_A6 + 0xf) = sVar4 + *(short *)(unaff_A6 + 0xf);
      candidate_427a();
      unaff_A6[0xf] = uVar2;
      uVar6 = 0;
    }
    else {
      candidate_427a();
    }
    if ((*(byte *)((int)unaff_A6 + 0x8d) & 1) != 0) {
      *unaff_A6 = candidate_23c44;
      uVar6 = 0;
    }
    candidate_4c24();
    if ((bool)uVar6) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar6 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_23e96();
      *(byte *)(unaff_A6[3] + 0x8f) =
           *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
      *unaff_A6 = candidate_24164;
      uVar6 = 0;
    }
    candidate_24c94();
    candidate_24a94();
    if ((bool)uVar6) {
      candidate_23e96();
      *(byte *)(unaff_A6[3] + 0x8f) =
           *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
      *unaff_A6 = candidate_240ee;
    }
    return;
  }
  candidate_23e96();
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  candidate_4256();
  *unaff_A6 = candidate_239ee;
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
  uVar6 = false;
  if ((*(char *)(unaff_A6[3] + 0x8e) != '\0') &&
     (bVar3 = *(byte *)(unaff_A6[3] + 0x8b), uVar6 = *(byte *)((int)unaff_A6 + 0x8b) < bVar3,
     *(byte *)((int)unaff_A6 + 0x8b) == bVar3)) {
    *unaff_A6 = candidate_23ae6;
    uVar6 = false;
  }
  candidate_4c24();
  if ((bool)uVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar6 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_24164;
    uVar6 = 0;
  }
  candidate_24c94();
  candidate_24a94();
  if ((bool)uVar6) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_240ee;
  }
  return;
}
