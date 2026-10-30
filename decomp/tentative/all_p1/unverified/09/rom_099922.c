/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x099922.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_99922(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar1;
  candidate_4256();
  *unaff_A6 = 0x99956;
  candidate_99bb2();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
  uVar2 = 0;
  if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar1;
    uVar2 = false;
    candidate_99cee();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_999ac;
      uVar2 = 0;
    }
  }
  candidate_99c3c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_999c2;
  }
  if (*(int *)((int)unaff_A6 + 0x76) == 0) {
    *unaff_A6 = candidate_99a22;
  }
  uVar2 = false;
  candidate_19504();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_99b22;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_99b22;
  }
  return;
}
