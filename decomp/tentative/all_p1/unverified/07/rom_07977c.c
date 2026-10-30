/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07977c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7977c(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_9f88();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_146e8();
  candidate_4256();
  *unaff_A6 = 0x797ba;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -8;
  candidate_427a();
  cVar1 = *(char *)(unaff_A6[3] + 0x8a);
  if (cVar1 == '\x01') {
    *unaff_A6 = candidate_797ee;
  }
  uVar2 = cVar1 == '\0';
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_5db92;
    uVar2 = 0;
  }
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
