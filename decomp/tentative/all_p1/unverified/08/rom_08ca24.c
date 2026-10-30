/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ca24.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ca24(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
  *unaff_A6 = 0x8ca2e;
  iVar1 = *(int *)((short)(*(short *)((int)unaff_A6 + 0x7e) * 4) + 0xe55ec);
  unaff_A6[0xd] = iVar1;
  if (iVar1 == -0x80000000) {
    *unaff_A6 = candidate_8ca5c;
  }
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 1;
  if ((*(byte *)((int)unaff_A6 + 0x85) & 2) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  candidate_1ac38();
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x01';
  return;
}
