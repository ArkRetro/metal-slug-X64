/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x023c44.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_23c44(void)

{
  undefined4 uVar1;
  ushort uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x25) = *(undefined2 *)((int)unaff_A6 + 0x92);
  candidate_24b1c();
  candidate_40bc();
  *unaff_A6 = 0x23c7e;
  if (*(short *)(unaff_A6 + 0x25) != 0) {
    *(short *)(unaff_A6 + 0x25) = *(short *)(unaff_A6 + 0x25) + -1;
  }
  candidate_1abc8();
  uVar4 = *(ushort *)(unaff_A6 + 0x24) < 3;
  if (*(ushort *)(unaff_A6 + 0x24) == 3) {
    uVar3 = *(short *)(unaff_A6[3] + 0x9a) * 4;
    unaff_A6[0x14] = *(undefined4 *)((short)uVar3 + 0xe0098);
    if (*(short *)(unaff_A6[3] + 0x9a) == 9) {
      uVar3 = uVar3 - 2;
    }
    uVar1 = unaff_A6[0xf];
    uVar2 = *(ushort *)(unaff_A6 + 0xf);
    bVar5 = CARRY2(uVar3,uVar2);
    *(ushort *)(unaff_A6 + 0xf) = uVar3 + uVar2;
    candidate_427a();
    if (bVar5) {
      uVar4 = 0;
      candidate_4256();
      candidate_24aec();
      if ((bool)uVar4) {
        if (*(short *)(unaff_A6 + 0x25) == 0) {
          *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
          *unaff_A6 = candidate_23dbc;
        }
        else {
          candidate_24b1c();
        }
      }
    }
    unaff_A6[0xf] = uVar1;
    uVar4 = 0;
  }
  else {
    candidate_427a();
    if ((bool)uVar4) {
      uVar4 = 0;
      candidate_4256();
      candidate_24aec();
      if ((bool)uVar4) {
        uVar4 = 0;
        if (*(short *)(unaff_A6 + 0x25) == 0) {
          *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
          *unaff_A6 = candidate_23dbc;
          uVar4 = 0;
        }
        else {
          candidate_24b1c();
        }
      }
    }
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_23e96();
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *unaff_A6 = candidate_24164;
    uVar4 = 0;
  }
  candidate_24c94();
  candidate_24a94();
  if ((bool)uVar4) {
    candidate_23e96();
    *(byte *)(unaff_A6[3] + 0x8f) =
         *(byte *)(unaff_A6[3] + 0x8f) | '\x01' << (*(ushort *)(unaff_A6 + 0x24) & 7);
    *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    *unaff_A6 = candidate_240ee;
  }
  return;
}
