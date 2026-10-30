/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a4c4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a4c4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x20) = 1;
  *unaff_A6 = 0x7a4e0;
  uVar1 = false;
  candidate_7b5c2();
  if ((bool)uVar1) {
    candidate_7b730();
  }
  candidate_1ac38();
  candidate_427a();
  candidate_7b79c();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7a97a;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_7b996();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7aae2;
  }
  return;
}
