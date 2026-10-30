/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c9fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c9fe(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  candidate_9ea8();
  *unaff_A6 = 0x7ca24;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x7c946;
    uVar1 = 0;
  }
  candidate_19504();
  if ((bool)uVar1) {
    *unaff_A6 = 0x7c946;
  }
  return;
}
