/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5ad4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_d5ad4(void)

{
  ushort uVar1;
  undefined2 uVar3;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_120f8();
  candidate_146e8();
  candidate_4520();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar3;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  candidate_1dc8e();
  bVar4 = false;
  if (*(char *)((int)unaff_A6 + 0x79) == '\0') {
    candidate_d7062();
    if (bVar4) {
      *(byte *)((int)unaff_A6 + 0x77) = *(byte *)((int)unaff_A6 + 0x77) ^ 1;
      unaff_A6[0xc] = 0;
      *(undefined2 *)((int)unaff_A6 + 0x72) = 0x3c;
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x96) = 0xe90d4;
      *unaff_A6 = 0xd5bf8;
      if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
        candidate_140c0();
      }
      candidate_1abc8();
      candidate_427a();
      *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
      if (*(short *)((int)unaff_A6 + 0x72) < 1) {
        *unaff_A6 = 0xd5b1e;
      }
      uVar5 = 0;
    }
    else {
      uVar1 = *(ushort *)(unaff_A6 + 0x2a);
      if ((*(byte *)((int)unaff_A6 + 0x77) & 1) == 0) {
        uVar1 = -uVar1;
      }
      unaff_A6[0xc] = (int)((uint)uVar1 << 0x10) >> 8;
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x96) = 0xe90c4;
      *unaff_A6 = 0xd5b7e;
      uVar5 = 0;
      if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
        candidate_140c0();
      }
      func_0x000143e4();
      candidate_d704e();
      if ((bool)uVar5) {
        *unaff_A6 = 0xd5bba;
      }
      if (unaff_A6[0xc] == 0) {
        *unaff_A6 = 0xd5bba;
      }
      uVar5 = 0;
      candidate_427a();
    }
  }
  else {
    uVar1 = *(short *)(unaff_A6 + 0x2a) >> 1;
    *(undefined1 *)((int)unaff_A6 + 0x77) = 1;
    if (*(short *)((int)unaff_A6 + 0x6e) < *(short *)(unaff_A6 + 10)) {
      uVar1 = -uVar1;
      *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
    }
    unaff_A6[0xc] = (int)((uint)uVar1 << 0x10) >> 8;
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x96) = 0xe90c4;
    *(undefined2 *)((int)unaff_A6 + 0x72) = 0xb4;
    *unaff_A6 = 0xd5c90;
    uVar5 = 0;
    if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
      candidate_140c0();
    }
    func_0x000143e4();
    candidate_d70cc();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_d5cd2;
    }
    candidate_427a();
    *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
    if (*(short *)((int)unaff_A6 + 0x72) < 1) {
      *unaff_A6 = candidate_d5cd2;
    }
    uVar5 = 0;
  }
  candidate_4c24();
  if ((((bool)uVar5) &&
      (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, *(char *)((int)unaff_A6 + 0x79) == '\0')) &&
     (*(short *)((int)unaff_A6 + 0x4e) <= *(short *)((int)unaff_A6 + 0x86))) {
    *(undefined1 *)((int)unaff_A6 + 0x79) = 1;
    candidate_1da40();
    candidate_120f8();
    *unaff_A6 = 0xd5c2c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x14;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  iVar2 = *(int *)((int)unaff_A6 + 0x96);
  if (iVar2 != 0) {
    iVar2 = candidate_49e8();
  }
  return iVar2;
}
