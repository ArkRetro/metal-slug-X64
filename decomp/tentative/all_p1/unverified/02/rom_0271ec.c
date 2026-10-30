/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0271ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_271ec(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar3;

  candidate_279f2();
  if (!(bool)in_CF) {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1d) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x62) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
    candidate_9ea8();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
    candidate_27820();
    candidate_4c1c();
    candidate_4256();
    *unaff_A6 = 0x2726c;
    *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
    if (*(short *)(unaff_A6 + 0x1d) < 1) {
      *unaff_A6 = candidate_27284;
    }
    candidate_1abc8();
    candidate_1ac38();
    *(byte *)(unaff_A6 + 0x21) = (byte)(*(char *)((int)unaff_A6 + 0x85) + 4U) >> 3;
    uVar3 = 0;
    candidate_427a();
    candidate_4c24();
    if ((bool)uVar3) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    sVar2 = candidate_49e8();
    if (sVar2 != 0) {
      *unaff_A6 = candidate_27616;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_27616;
    }
    uVar3 = false;
    candidate_19424();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_27616;
      uVar3 = 0;
    }
    candidate_1945c();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1d) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x62) = uVar1;
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_27820();
  candidate_4256();
  *unaff_A6 = 0x274c4;
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_274dc;
  }
  candidate_1abc8();
  candidate_1ac38();
  *(byte *)(unaff_A6 + 0x21) = (byte)(*(char *)((int)unaff_A6 + 0x85) + 4U) >> 3;
  candidate_427a();
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  if ('\x02' < *(char *)(unaff_A6 + 0x23)) {
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)((int)unaff_A6 + (short)(*(byte *)(unaff_A6 + 0x23) & 3) + 0x19);
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_27616;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_27616;
  }
  uVar3 = false;
  candidate_19424();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_27616;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
