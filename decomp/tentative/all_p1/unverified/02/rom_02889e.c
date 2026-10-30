/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02889e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2889e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(byte *)((int)unaff_A6 + 0x7d) = *(byte *)((int)unaff_A6 + 0x7d) & 0xef;
  candidate_4256();
  *unaff_A6 = 0x288ba;
  uVar1 = 0;
  candidate_28e2c();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_288d2;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_289a6;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
