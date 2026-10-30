/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020f88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20f88(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  undefined4 uVar2;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x40;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x40;
  candidate_9f88();
  uVar1 = (uint)(ushort)(*(short *)((int)unaff_A6 + 0x72) << 2);
  candidate_1ab98();
  uVar2 = *(undefined4 *)((short)uVar1 + 0x2071e);
  candidate_9ea8(uVar2,uVar1);
  candidate_9ea8(uVar2);
  candidate_9ea8();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0;
  *unaff_A6 = candidate_21026;
  return;
}
