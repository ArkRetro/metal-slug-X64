/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021bb2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21bb2(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_1456a();
  candidate_427a();
  if (*(char *)(unaff_A6 + 0x20) == '\x01') {
    *unaff_A6 = candidate_21e9a;
  }
  bVar1 = *(char *)(unaff_A6 + 0x20) == '\0';
  candidate_4c24();
  if (bVar1) {
    if (*(char *)((int)unaff_A6 + 0x5a) == '\x10') {
      *(char *)((int)unaff_A6 + 0x69) = (char)*(undefined2 *)((int)unaff_A6 + 0x7e);
      *unaff_A6 = candidate_21c0a;
    }
    else {
      *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    }
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  candidate_220bc();
  return;
}
