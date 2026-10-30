/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d296.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d296(void)

{
  int iVar1;
  int unaff_A6;
  bool bVar2;
  undefined8 uVar3;

  *(undefined1 *)(unaff_A6 + 0x7e) = 0;
  *(undefined1 *)(unaff_A6 + 0x7f) = 0;
  *(undefined1 *)(unaff_A6 + 0x83) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x83);
  *(undefined1 *)(unaff_A6 + 0x80) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x80);
  candidate_1ad6e();
  uVar3 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x30) = uVar3;
  func_0x000143e4();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x80) == 0) {
    candidate_2e7c();
    bVar2 = false;
    candidate_14906();
    if (bVar2) {
      *(undefined1 *)(unaff_A6 + 0x7f) = 0xff;
    }
    else {
      *(char *)(unaff_A6 + 0x7e) = *(char *)(unaff_A6 + 0x7e) + -2;
    }
  }
  else {
    *(char *)(unaff_A6 + 0x7e) = *(char *)(unaff_A6 + 0x7e) + '\x02';
    *(undefined1 *)(unaff_A6 + 0x7f) = 0xff;
  }
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined1 *)(iVar1 + 0x7e) = *(undefined1 *)(unaff_A6 + 0x7e);
  *(undefined1 *)(iVar1 + 0x7f) = *(undefined1 *)(unaff_A6 + 0x7f);
  return;
}
