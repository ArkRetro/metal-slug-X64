/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x022cb6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_22cb6(void)

{
  int iVar1;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9f88();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x9a) = 0;
  *unaff_A6 = 0x22d08;
  candidate_4256();
  if ((*(byte *)(unaff_A6[3] + 0x8d) & 2) == 0) {
    unaff_A6[0x21] = 0x224e2;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
  }
  else {
    unaff_A6[0x21] = 0x225a6;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0;
  *(undefined4 *)(unaff_A5 + 0x694e) = 0;
  *(undefined4 *)(unaff_A5 + 0x6952) = 0;
  *unaff_A6 = 0x22d6c;
  candidate_1abc8();
  candidate_427a();
  candidate_234de();
  iVar1 = unaff_A6[3];
  *(undefined2 *)(iVar1 + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  if (*(char *)(iVar1 + 0x8c) == '\x01') {
    *unaff_A6 = candidate_22e38;
  }
  if (*(char *)(iVar1 + 0x8c) == '\x02') {
    *unaff_A6 = candidate_22e38;
  }
  if (*(char *)(iVar1 + 0x8c) == '\x03') {
    *unaff_A6 = candidate_2300a;
  }
  if ((*(char *)(iVar1 + 0x8b) == '\a') && (*(char *)(iVar1 + 0x8e) != '\0')) {
    if ((*(byte *)(iVar1 + 0x8d) & 2) == 0) {
      *unaff_A6 = candidate_231a6;
    }
    else if (*(char *)(iVar1 + 0x8e) != '\x7f') {
      *unaff_A6 = 0x22d08;
    }
  }
  if ((*(char *)(iVar1 + 0x8b) == '\b') && (*(char *)(iVar1 + 0x8e) != '\0')) {
    if ((*(byte *)(iVar1 + 0x8d) & 1) == 0) {
      *unaff_A6 = candidate_232ec;
    }
    else {
      *unaff_A6 = 0x22d08;
    }
  }
  if (((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) && ((*(byte *)(unaff_A6[3] + 0x8d) & 4) != 0)) {
    *unaff_A6 = candidate_23432;
  }
  return;
}
