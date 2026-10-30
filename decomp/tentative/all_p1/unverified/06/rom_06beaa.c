/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06beaa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6beaa(void)

{
  short sVar1;
  byte bVar2;
  byte bVar3;
  undefined1 uVar6;
  undefined2 uVar4;
  ushort uVar5;
  undefined4 *unaff_A6;

  candidate_6cc68();
  uVar6 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x8e) = uVar6;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  *unaff_A6 = 0x6bec8;
  sVar1 = *(short *)((int)unaff_A6 + 0x86);
  uVar6 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar1;
  if (sVar1 < 0) {
    uVar4 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x86) = uVar4;
    candidate_6cb76();
    bVar2 = *(byte *)((int)unaff_A6 + 0x8e);
    uVar6 = bVar2 == 0;
    bVar3 = bVar2 - 1;
    *(byte *)((int)unaff_A6 + 0x8e) = bVar3;
    if (bVar3 == 0 || SBORROW1(bVar2,'\x01') != (short)((ushort)bVar3 << 8) < 0) {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
      *unaff_A6 = 0x6bf20;
      uVar6 = 0;
      candidate_6ca80();
      candidate_427a();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_6be00;
        uVar6 = 0;
      }
      goto LAB_0006c1f2;
    }
  }
  candidate_6ca5c();
  candidate_6ca80();
  candidate_427a();
LAB_0006c1f2:
  candidate_4c24();
  if ((bool)uVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
    uVar5 = candidate_260c8();
    uVar6 = uVar5 < *(ushort *)((int)unaff_A6 + 0x4e);
    if ((short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)uVar5) {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xfd;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6c0d8;
    uVar6 = 0;
  }
  candidate_1945c();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_6c14c;
  }
  candidate_140c0();
  return;
}
