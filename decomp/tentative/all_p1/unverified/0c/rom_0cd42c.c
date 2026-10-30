/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd42c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd42c(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  uVar1 = candidate_260c8();
  *(ushort *)((int)unaff_A6 + 0x4e) = uVar1;
  *(ushort *)(unaff_A6 + 0x1e) = uVar1 >> 2;
  *(ushort *)((int)unaff_A6 + 0x76) = uVar1 - (uVar1 >> 2);
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x7fe;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x7fe;
  candidate_9ea8();
  candidate_4520();
  *(undefined1 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7f) = 0;
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0xcd4b8;
  candidate_1abc8();
  *(short *)(unaff_A6 + 10) = (short)*(char *)((int)unaff_A6 + 0x7a);
  *(short *)(unaff_A6 + 0xb) = (short)*(char *)((int)unaff_A6 + 0x7b);
  bVar2 = false;
  candidate_427a();
  if (bVar2) {
    *unaff_A6 = candidate_cd500;
    if (3 < *(byte *)((int)unaff_A6 + 0x7e)) {
      *unaff_A6 = candidate_cd70a;
    }
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  }
  return;
}
