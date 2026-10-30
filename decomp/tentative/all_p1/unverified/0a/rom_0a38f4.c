/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a38f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a38f4(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_a3dfc();
  candidate_1ad6e();
  candidate_4520();
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  candidate_4256();
  *unaff_A6 = 0xa3932;
  candidate_a3ec0();
  *(undefined2 *)((int)unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6[3] + 0x26);
  uVar2 = 0;
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    if (*(char *)((int)unaff_A6 + 0x93) == '\x01') {
      candidate_49e8();
      candidate_49e8();
    }
    uVar2 = *(byte *)((int)unaff_A6 + 0x93) < 2;
    if (*(byte *)((int)unaff_A6 + 0x93) == 2) {
      candidate_49e8();
      candidate_49e8();
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (((*(char *)(unaff_A6 + 0x18) != '3') && (*(char *)(unaff_A6 + 0x18) != 'w')) &&
     (*(short *)((int)unaff_A6 + 0x4e) < 1)) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0x33;
    candidate_120f8();
    *(undefined4 *)unaff_A6[3] = 0xa35a8;
  }
  if (*(byte *)(unaff_A6 + 0x18) == 0x77) {
    *unaff_A6 = candidate_a39fe;
  }
  bVar3 = *(byte *)(unaff_A6 + 0x18) < 0x77;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
