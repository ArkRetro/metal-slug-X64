/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097d22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_97d22(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x97d52;
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  if (*(char *)(iVar1 + 0x91) == '\x06') {
    if (*(char *)((int)unaff_A6 + 0x91) == '\0') {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 1;
    }
    else {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  }
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  candidate_427a();
  if (*(short *)(unaff_A6[3] + 0x76) != *(short *)((int)unaff_A6 + 0x76)) {
    *unaff_A6 = 0x97c72;
  }
  return;
}
