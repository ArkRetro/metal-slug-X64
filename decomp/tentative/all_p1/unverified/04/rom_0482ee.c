/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0482ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_482ee(void)

{
  char cVar1;
  undefined1 uVar2;
  int extraout_A0;
  undefined4 *unaff_A6;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  uVar2 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x26) = uVar2;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x97) = uVar2;
  *(undefined1 *)(unaff_A6 + 0x25) = *(undefined1 *)((int)unaff_A6 + 0x97);
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x99) = uVar2;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x9a) = uVar2;
  *unaff_A6 = 0x48342;
  if (*(char *)((int)unaff_A6 + 0x9a) == '\0') {
    if (*(char *)((int)unaff_A6 + 0x97) == '\0') {
      uVar2 = candidate_260c8();
      *(undefined1 *)((int)unaff_A6 + 0x97) = uVar2;
      candidate_40bc();
      *(undefined1 *)(extraout_A0 + 0x60) = 0;
      *(undefined2 *)(unaff_A6 + 0x20) = 0xc0;
      cVar1 = *(char *)(unaff_A6 + 0x26) + -1;
      *(char *)(unaff_A6 + 0x26) = cVar1;
      if (cVar1 == '\0') {
        uVar2 = candidate_260c8();
        *(undefined1 *)(unaff_A6 + 0x26) = uVar2;
        uVar2 = candidate_260c8();
        *(undefined1 *)((int)unaff_A6 + 0x9a) = uVar2;
      }
    }
    else {
      *(char *)((int)unaff_A6 + 0x97) = *(char *)((int)unaff_A6 + 0x97) + -1;
    }
  }
  else {
    *(char *)((int)unaff_A6 + 0x9a) = *(char *)((int)unaff_A6 + 0x9a) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x99) == '\0') {
    *unaff_A6 = candidate_483e0;
  }
  else {
    *(char *)((int)unaff_A6 + 0x99) = *(char *)((int)unaff_A6 + 0x99) + -1;
  }
  candidate_49e9e();
  func_0x000143e4();
  candidate_427a();
  candidate_4900c();
  return;
}
