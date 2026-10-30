/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a86c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a86c(void)

{
  uint uVar1;
  undefined4 uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9f88();
  candidate_9ea8();
  uVar2 = 0xfffd999a;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar2 = 0x26666;
  }
  unaff_A6[0xc] = uVar2;
  unaff_A6[0xd] = 0x3fffc;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff3334;
  unaff_A6[0xd] = -unaff_A6[0xd];
  *(int *)((int)unaff_A6 + 0x72) = -*(int *)((int)unaff_A6 + 0x72);
  uVar3 = candidate_29c6();
  *(int *)((int)unaff_A6 + 0x72) =
       (short)((uVar3 & 0x3f) - 0x1f) * 0x100 + *(int *)((int)unaff_A6 + 0x72);
  candidate_4256();
  *unaff_A6 = 0x7a94c;
  uVar1 = unaff_A6[0xd];
  uVar4 = CARRY4(*(uint *)((int)unaff_A6 + 0x72),uVar1);
  unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x72) + uVar1;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = 0x7aae8;
    uVar4 = 0;
  }
  candidate_7b9a4();
  if ((bool)uVar4) {
    *unaff_A6 = 0x7aae8;
  }
  return;
}
