/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0469e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_469e8(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_4256();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  candidate_1ad6e();
  *unaff_A6 = 0x46a1c;
  iVar1 = unaff_A6[3];
  *(undefined1 *)((int)unaff_A6 + 0x91) = *(undefined1 *)(iVar1 + 0x91);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  if (*(char *)(iVar1 + 0x92) != '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x10;
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
