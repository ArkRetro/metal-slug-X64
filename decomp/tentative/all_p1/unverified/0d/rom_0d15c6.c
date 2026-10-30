/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d15c6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_d15c6(void)

{
  ushort uVar1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff0;
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x7fff;
  unaff_A6[0x14] = 0xd1222;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_4256();
  candidate_1ad6e();
  *unaff_A6 = 0xd1692;
  uVar3 = 0;
  candidate_d1b46();
  candidate_d1b28();
  uVar1 = candidate_19424();
  if (((!(bool)uVar3) && (*(short *)(unaff_A5 + 0x699e) == 0x1000)) &&
     (uVar1 = _DAT_0010000e & 0xff1f, (_DAT_0010000e & 0x1f) == 0)) {
    uVar1 = candidate_29c6();
    uVar1 = uVar1 & 3;
    if (uVar1 == 0) {
      *unaff_A6 = candidate_d16dc;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return uVar1;
}
