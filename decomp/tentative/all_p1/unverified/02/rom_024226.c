/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024226.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24226(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x3000;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = *(undefined2 *)((int)unaff_A6 + 0x4e);
  candidate_1ad6e();
  if ((*(byte *)(unaff_A6[3] + 0x8d) & 1) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  unaff_A6[0x14] = 0;
  *unaff_A6 = 0x242d2;
  candidate_1abc8();
  candidate_427a();
  iVar1 = unaff_A6[3];
  uVar2 = false;
  if (*(char *)(iVar1 + 0x8e) != '\0') {
    if (*(char *)((int)unaff_A6 + 0x8b) == *(char *)(iVar1 + 0x8b)) {
      if ((*(byte *)(iVar1 + 0x8d) & 1) == 0) {
        if ((*(byte *)(iVar1 + 0x8d) & 2) != 0) {
          *unaff_A6 = candidate_24358;
        }
      }
      else {
        *unaff_A6 = candidate_243e2;
      }
    }
    uVar2 = *(byte *)(iVar1 + 0x8b) < 8;
    if ((*(byte *)(iVar1 + 0x8b) == 8) && ((*(byte *)(iVar1 + 0x8d) & 1) != 0)) {
      *unaff_A6 = candidate_2458c;
      uVar2 = false;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  candidate_24cca();
  candidate_24ab0();
  if ((bool)uVar2) {
    unaff_A6[0x14] = 0;
  }
  return;
}
