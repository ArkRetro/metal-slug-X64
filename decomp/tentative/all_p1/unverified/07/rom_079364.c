/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079364.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79364(void)

{
  int iVar1;
  char cVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_1ad6e();
  *unaff_A6 = 0x793ac;
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  *(undefined1 *)((int)unaff_A6 + 0x8b) = *(undefined1 *)(iVar1 + 0x8b);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + 8;
  cVar2 = *(char *)(iVar1 + 0x8a);
  if (cVar2 == '\x01') {
    *unaff_A6 = candidate_793ec;
  }
  uVar3 = cVar2 == '\0';
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
