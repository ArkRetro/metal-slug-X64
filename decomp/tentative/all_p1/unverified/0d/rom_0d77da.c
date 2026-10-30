/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d77da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d77da(void)

{
  undefined2 uVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x7f) = uVar2;
  *(byte *)((int)unaff_A6 + 0x61) = *(byte *)((int)unaff_A6 + 0x61) | 0x10;
  while( true ) {
    *unaff_A6 = 0xd785e;
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
    if ((0 < *(short *)((int)unaff_A6 + 0x76)) || ((*(byte *)((int)unaff_A6 + 0x61) & 0x10) == 0))
    goto LAB_000d7884;
    if (*(char *)((int)unaff_A6 + 0x7f) < '\x01') break;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *(char *)((int)unaff_A6 + 0x7f) = *(char *)((int)unaff_A6 + 0x7f) + -1;
    *(undefined1 *)(unaff_A6 + 0x22) = 0x86;
    *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x1e);
  }
  *unaff_A6 = candidate_d7892;
LAB_000d7884:
  candidate_1abc8();
  candidate_d90f8();
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (((*(short *)(unaff_A6 + 0xf) < 0x90) ||
        (bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x120, 0x120 < (short)*(ushort *)(unaff_A6 + 0xf)))
       || (candidate_1af9a(), bVar3)) {
      *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    }
    else {
      *unaff_A6 = candidate_d7d7c;
    }
  }
  return;
}
