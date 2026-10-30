/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ce42.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_8ce42(void)

{
  undefined4 *puVar1;
  short in_D0w;
  undefined4 in_A0;
  int extraout_A0;
  undefined2 extraout_A0w;
  undefined2 uVar2;
  undefined2 uStack_2;

  uVar2 = (undefined2)((uint)in_A0 >> 0x10);
  uStack_2 = (undefined2)in_A0;
  candidate_120f8();
  if (in_D0w == 3) {
    in_D0w = 0;
  }
  puVar1 = (undefined4 *)((short)(in_D0w << 2) + 0x10c5a0);
  *puVar1 = *(undefined4 *)CONCAT22(uVar2,uStack_2);
  candidate_411c((short)puVar1);
  *(char *)(extraout_A0 + 0x8a) = (char)in_D0w;
  *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((short)(in_D0w * 4) + 0x10c5a0);
  candidate_1acf2();
  if (in_D0w != 2) {
    candidate_57948(extraout_A0w);
  }
  return in_D0w;
}
