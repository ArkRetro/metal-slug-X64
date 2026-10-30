/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6580.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c6580(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar1;

  candidate_c6a46();
  if (((bool)in_CF) &&
     (in_CF = *(char *)((int)unaff_A6 + 0x97) == '\0', *(char *)((int)unaff_A6 + 0x97) == '\x01')) {
    *(undefined1 *)((int)unaff_A6 + 0x97) = 2;
    in_CF = 0;
  }
  candidate_c6a02();
  candidate_c6a1e();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x97) = 2;
  }
  candidate_427a();
  candidate_c699c();
  candidate_c6b4e();
  if (*(byte *)((int)unaff_A6 + 0x97) == 2) {
    *unaff_A6 = candidate_c660e;
  }
  uVar1 = *(byte *)((int)unaff_A6 + 0x97) < 2;
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_c5d26;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_c5dcc;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
