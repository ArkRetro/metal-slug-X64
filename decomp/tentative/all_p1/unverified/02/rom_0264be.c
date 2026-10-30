/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0264be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_264be(void)

{
  short sVar1;
  uint uVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  do {
    candidate_27a32();
    if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x80) == 0) {
      uVar3 = candidate_260c8();
    }
    else {
      uVar3 = candidate_260c8();
    }
    *(undefined2 *)(unaff_A6 + 0x1d) = uVar3;
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_27848();
    candidate_4256();
    *unaff_A6 = 0x26514;
    sVar1 = *(short *)(unaff_A6 + 0x1d);
    bVar4 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x1d) = sVar1 + -1;
    candidate_278c4();
    if ((!bVar4) || (0 < *(short *)(unaff_A6 + 0x1d))) {
      candidate_1abc8();
      *(short *)(unaff_A6 + 0xb) = (0xf0 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb)
      ;
      candidate_1abc8();
      candidate_49e8();
      goto LAB_00026b82;
    }
    uVar2 = candidate_27936();
    switch(uVar2 & 3) {
    case 1:
      candidate_27848();
      candidate_27a32();
      *(undefined2 *)(unaff_A6 + 0x1d) = 0xf;
      *unaff_A6 = candidate_26a0a;
      candidate_1abc8();
      *(short *)(unaff_A6 + 0xb) = (0xf0 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb)
      ;
      candidate_1abc8();
      *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
      if (*(short *)(unaff_A6 + 0x1d) < 1) {
        *unaff_A6 = candidate_26a3e;
      }
      candidate_278c4();
LAB_00026b78:
      candidate_49e8();
LAB_00026b82:
      candidate_427a();
      if (*(short *)((int)unaff_A6 + 0x4e) < *(short *)((int)unaff_A6 + 0x7e)) {
        *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x80;
      }
      if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x10) == 0) {
        _DAT_0010e9ba = 0x1a0;
      }
      else {
        _DAT_0010e9ba = 0x110;
      }
      candidate_19388();
      if (_DAT_0010e99a == 0) {
        *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) & 0xf7;
        *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
      }
      else {
        *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 8;
        *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + '\x01';
      }
      candidate_4c24();
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      if (0x7e00 < _DAT_0010e99e) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        *unaff_A6 = candidate_26c30;
      }
      candidate_27bd4();
      *(undefined1 *)((int)unaff_A6 + 0x8e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
      *(undefined1 *)((int)unaff_A6 + 0x5e) = 0;
      return;
    case 2:
      candidate_27a32();
      candidate_27848();
      if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x80) == 0) {
        uVar3 = 0x1e;
      }
      else {
        uVar3 = 0xf;
      }
      *(undefined2 *)(unaff_A6 + 0x1d) = uVar3;
      candidate_1abc8();
      *(short *)(unaff_A6 + 0xb) = (0xf0 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb)
      ;
      candidate_1abc8();
      *unaff_A6 = candidate_26594;
      uVar2 = candidate_29c6();
      if ((uVar2 & 1) == 0) {
        if (*(short *)(unaff_A6 + 0x10) < 0xd0) {
          *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 2;
          *(short *)(unaff_A6 + 0x10) = *(short *)(unaff_A6 + 0x10) + 2;
        }
        else {
          *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -2;
          *(short *)(unaff_A6 + 0x10) = *(short *)(unaff_A6 + 0x10) + -2;
        }
      }
      *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
      if (*(short *)(unaff_A6 + 0x1d) < 1) {
        *unaff_A6 = candidate_265e2;
      }
      candidate_49e8();
      goto LAB_00026b82;
    case 3:
      candidate_27a32();
      if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x80) == 0) {
        uVar3 = candidate_260c8();
      }
      else {
        uVar3 = candidate_260c8();
      }
      *(undefined2 *)(unaff_A6 + 0x1d) = uVar3;
      candidate_4256();
      candidate_120f8();
      *unaff_A6 = candidate_2684c;
      candidate_1abc8();
      *(short *)(unaff_A6 + 0xb) = (0xf0 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb)
      ;
      candidate_1abc8();
      *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
      if (*(short *)(unaff_A6 + 0x1d) < 1) {
        *unaff_A6 = candidate_2688c;
      }
      candidate_27640();
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x17) = 0;
      goto LAB_00026b78;
    }
  } while( true );
}
