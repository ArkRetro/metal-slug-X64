/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a9d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a9d2(void)

{
  undefined4 uVar1;
  int extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;
  bool bVar2;

  uVar1 = 0x26666;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar1 = 0xfffd999a;
  }
  unaff_A6[0xc] = uVar1;
  unaff_A6[0xd] = 0xfffa;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffffe667;
  candidate_120f8();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0x7aa0c;
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  candidate_1ac38();
  candidate_427a();
  bVar2 = *(ushort *)(unaff_A6 + 10) < 0xb40;
  if ((0xb3f < (short)*(ushort *)(unaff_A6 + 10)) &&
     (bVar2 = false, *(char *)((int)unaff_A6 + 0x61) == '\0')) {
    candidate_b20f4();
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    candidate_1da40();
    *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
    candidate_1da40();
    *(undefined2 *)(extraout_A0_00 + 0x66) = 0x1001;
    *(undefined2 *)(extraout_A0_00 + 0x1e) = 0x1001;
    bVar2 = false;
  }
  candidate_7b996();
  if (bVar2) {
    *unaff_A6 = candidate_7aae2;
  }
  return;
}
