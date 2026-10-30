/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0753d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_753d6(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  unaff_A6[0x14] = 0xe3ac0;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  candidate_766e0();
  candidate_40bc();
  *unaff_A6 = 0x7542e;
  uVar1 = 0;
  candidate_767b0();
  candidate_21288();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_751ec;
    uVar1 = 0;
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_76802();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_751ec;
  }
  return;
}
