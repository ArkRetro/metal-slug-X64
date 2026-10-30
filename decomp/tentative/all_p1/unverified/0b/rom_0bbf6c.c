/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bbf6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bbf6c(void)

{
  char cVar1;
  undefined2 uVar2;
  int extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  uVar3 = false;
  if (*(short *)((int)unaff_A6 + 0x4e) < 1) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + 1;
    candidate_4256();
    uVar3 = *(ushort *)((int)unaff_A6 + 0x76) < 3;
    if (!(bool)uVar3) {
      *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
      bVar4 = false;
      candidate_40bc();
      if (bVar4) {
        candidate_1b220();
      }
      bVar4 = false;
      candidate_40bc();
      if (bVar4) {
        candidate_1b220();
      }
      candidate_120f8();
      candidate_1da40();
      *(undefined2 *)(extraout_A0 + 0x66) = 0xfff2;
      *(undefined2 *)(extraout_A0 + 0x1e) = 0xfff2;
      candidate_ca2e0();
      candidate_ca2e0();
      *(undefined1 *)((int)unaff_A6 + 0x7b) = 0x32;
      *unaff_A6 = 0xbc068;
      cVar1 = *(char *)((int)unaff_A6 + 0x7b) + -1;
      *(char *)((int)unaff_A6 + 0x7b) = cVar1;
      if (-1 < cVar1) {
        candidate_1abc8();
        return;
      }
      candidate_9f88();
      candidate_9ea8();
      candidate_4256();
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
      candidate_c88b0();
      candidate_40bc();
      *(undefined4 *)(extraout_A0_00 + 0x38) = 0x10e968;
      *(undefined4 *)(extraout_A0_00 + 0x28) = 0x7800000;
      *(undefined4 *)(extraout_A0_00 + 0x2c) = 0x280000;
      *(undefined4 *)(extraout_A0_00 + 0x6e) = 0xbbaec;
      *(undefined2 *)(extraout_A0_00 + 0x72) = 0x788;
      *unaff_A6 = 0xbc0ee;
      *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
      candidate_1abc8();
      candidate_427a();
      return;
    }
  }
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  return;
}
