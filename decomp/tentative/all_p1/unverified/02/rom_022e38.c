/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x022e38.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_22e38(void)

{
  int iVar1;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_120f8();
  if ((*(byte *)(unaff_A6[3] + 0x8d) & 2) == 0) {
    unaff_A6[0x21] = 0x224f6;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
  }
  else {
    unaff_A6[0x21] = 0x225ba;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
  }
  unaff_A6[0x1f] = 0x1000;
  unaff_A6[0x20] = 0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x96) = 0;
  *unaff_A6 = 0x22e96;
  candidate_1abc8();
  candidate_427a();
  candidate_234de();
  iVar1 = unaff_A6[3];
  *(undefined2 *)(iVar1 + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  if (*(char *)(iVar1 + 0x8c) == '\x02') {
    unaff_A6[0xc] = unaff_A6[0x1f] + unaff_A6[0xc];
    if (0x28000 < (int)unaff_A6[0xc]) {
      unaff_A6[0xc] = 0x28000;
    }
  }
  else if (unaff_A6[0xc] != 0x18000) {
    iVar1 = unaff_A6[0x1f];
    if (0x18000 < (int)unaff_A6[0xc]) {
      iVar1 = -iVar1;
    }
    unaff_A6[0xc] = iVar1 + unaff_A6[0xc];
  }
  *(undefined4 *)(unaff_A5 + 0x694e) = unaff_A6[0xc];
  *(undefined4 *)(unaff_A5 + 0x6952) = unaff_A6[0xd];
  iVar1 = unaff_A6[3];
  if ((*(char *)(iVar1 + 0x8c) != '\x01') && (*(char *)(iVar1 + 0x8c) != '\x02')) {
    *unaff_A6 = candidate_22f70;
  }
  if ((((*(char *)(iVar1 + 0x8b) == '\a') && (*(char *)(iVar1 + 0x8e) != '\0')) &&
      (*(char *)(iVar1 + 0x8e) != '\x7f')) && ((*(byte *)(iVar1 + 0x8d) & 2) == 0)) {
    *unaff_A6 = candidate_22f70;
  }
  if ((*(char *)(iVar1 + 0x8b) == '\b') && (*(char *)(iVar1 + 0x8e) != '\0')) {
    *unaff_A6 = candidate_22f70;
  }
  candidate_234f6();
  return;
}
