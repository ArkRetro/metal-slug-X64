/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2012.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2012(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4520();
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  candidate_4256();
  *unaff_A6 = 0xd206e;
  uVar2 = 0;
  candidate_d3268();
  candidate_d31fc();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d20aa;
    uVar2 = 0;
  }
  candidate_d341e();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_d20aa;
  }
  return;
}
