# How to Use the Project Issue Template

This guide explains how to create issues using the provided template, link them to GitHub Projects, Milestones, and more.

## 1. Creating an Issue

When you create a new issue, choose the **Project Issue / Feature Request / Bug Report** template. Fill out each section as follows:

### Example

```
[Feature Request] Add Dark Mode Support

## Issue Type

- [x] Feature Request
- [ ] Bug
- [ ] Task
- [ ] Improvement

## Description

Implement a dark mode theme for the application UI.

## Steps to Reproduce (if Bug)
(n/a)

## Expected Behavior

The user can toggle between light and dark themes in the settings.

## Actual Behavior

Currently, only the light theme is available.

## Additional Context / Screenshots

n/a

## Acceptance Criteria

- [ ] Toggle in settings enables dark mode
- [ ] UI colors adjust for dark mode
- [ ] User preference is saved

## Related Issues / PRs

n/a

## Project / Milestone / Labels

- **Project:** Next UI Release
- **Milestone:** v2.0.0
- **Labels:** enhancement, UI

## Assignees

Assign to: @janedoe
```

## 2. Linking to Projects, Milestones, Labels, and Assignees

- **Project**: In the GitHub issue sidebar, use the "Projects" section to link the issue to a Project. You can also mention it in the template body for clarity.
- **Milestone**: Select the relevant milestone from the sidebar or mention in the template.
- **Labels**: Add appropriate labels via the sidebar or in the template.
- **Assignees**: Assign team members directly or mention them in the template.

## 3. Tips

- Always keep the issue title clear and concise, starting with the type (e.g. [Bug], [Task], [Feature Request]).
- If your issue is a bug, fill out the reproduction steps and actual/expected behavior.
- Use the "Acceptance Criteria" checklist to define when the issue is considered done.
- Link related issues or PRs using #issue_number or #pr_number.

## 4. Creating Issues Automatically (via CLI or API)

You can also create issues using this template via GitHub CLI or API. Example using GitHub CLI:

```bash
gh issue create --title "[Bug] Login fails on Safari" --body "$(cat path_to_template.md)" --label bug --assignee janedoe --project "Your Project Name" --milestone "v2.0.0"
```

Make sure to fill in the template placeholders before running the command.

---

Feel free to customize the template and documentation as needed for your project!