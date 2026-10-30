/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d65ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d65ee(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  *(undefined4 *)((int)unaff_A6 + 0x9a) = 0xe95a6;
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  *unaff_A6 = 0xd6634;
  uVar2 = 0;
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_140c0();
  }
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_d668e;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d678e;
  }
  if (*(char *)(unaff_A6[3] + 0x7a) == '\x05') {
    *unaff_A6 = 0xd6792;
  }
  return;
}
